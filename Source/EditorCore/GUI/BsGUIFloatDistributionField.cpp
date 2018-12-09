//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2018 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "GUI/BsGUIFloatDistributionField.h"
#include "GUI/BsGUILayout.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUILabel.h"
#include "GUI/BsGUIColor.h"
#include "GUI/BsGUIFloatField.h"
#include "GUI/BsGUIVector2Field.h"
#include "GUI/BsGUIVector3Field.h"
#include "GUI/BsGUICurves.h"
#include "GUI/BsGUIButton.h"
#include "GUI/BsGUIContextMenu.h"
#include "GUI/BsGUISpace.h"

using namespace std::placeholders;

namespace bs
{
	namespace impl
	{
		template<class T>
		bool isVertical() { return true; }

		template<>
		bool isVertical<float>() { return false; }
	}

	template<class T, class SELF>
	TGUIDistributionField<T, SELF>::TGUIDistributionField(const PrivatelyConstruct& dummy, const GUIContent& labelContent, 
		UINT32 labelWidth, const String& style, const GUIDimensions& dimensions, bool withLabel)
		: TGUIField(dummy, labelContent, labelWidth, style, dimensions, withLabel)
	{
		mContextMenu = bs_shared_ptr_new<GUIContextMenu>();

		mContextMenu->addMenuItem("Constant", [this]()
		{
			mValue = TDistribution<T>(mMinConstant);
			rebuild();
		}, 50);

		mContextMenu->addMenuItem("Range", [this]()
		{
			mValue = TDistribution<T>(mMinConstant, mMaxConstant);
			rebuild();
		}, 40);

		mContextMenu->addMenuItem("Curve", [this]()
		{
			TAnimationCurve<T> combinedCurve;
			AnimationUtility::combineCurve<T>(mMinCurve, combinedCurve);

			mValue = TDistribution<T>(combinedCurve);
			rebuild();
		}, 30);

		mContextMenu->addMenuItem("Curve range", [this]()
		{
			TAnimationCurve<T> combinedCurveMin;
			AnimationUtility::combineCurve<T>(mMinCurve, combinedCurveMin);

			TAnimationCurve<T> combinedCurveMax;
			AnimationUtility::combineCurve<T>(mMaxCurve, combinedCurveMax);

			mValue = TDistribution<T>(combinedCurveMin, combinedCurveMax);
			rebuild();
		}, 20);

		rebuild();
	}

	template<class T, class SELF>
	void TGUIDistributionField<T, SELF>::setValue(const TDistribution<T>& value)
	{
		mValue = value;

		switch (mValue.getType())
		{
		default:
		case PDT_Constant:
			mMinConstant = mValue.getMinConstant();
			mMaxConstant = mMinConstant;

			for(UINT32 i = 0; i < NumComponents; i++)
			{
				mMinCurve[i] = TAnimationCurve<float>({
					{ TCurveProperties<T>::getComponent(mMinConstant, i), 0.0f, 0.0f, 0.0f},
					{ TCurveProperties<T>::getComponent(mMinConstant, i), 0.0f, 0.0f, 1.0f} });

				mMaxCurve[1] = TAnimationCurve<float>({
					{ TCurveProperties<T>::getComponent(mMinConstant, i), 0.0f, 0.0f, 0.0f},
					{ TCurveProperties<T>::getComponent(mMinConstant, i), 0.0f, 0.0f, 1.0f} });
			}

			break;
		case PDT_RandomRange: 
			mMinConstant = mValue.getMinConstant();
			mMaxConstant = mValue.getMaxConstant();

			for(UINT32 i = 0; i < NumComponents; i++)
			{
				mMinCurve[i] = TAnimationCurve<float>({
					{ TCurveProperties<T>::getComponent(mMinConstant, i), 0.0f, 0.0f, 0.0f},
					{ TCurveProperties<T>::getComponent(mMinConstant, i), 0.0f, 0.0f, 1.0f} });

				mMaxCurve[1] = TAnimationCurve<float>({
					{ TCurveProperties<T>::getComponent(mMaxConstant, i), 0.0f, 0.0f, 0.0f},
					{ TCurveProperties<T>::getComponent(mMaxConstant, i), 0.0f, 0.0f, 1.0f} });
			}
			break;
		case PDT_Curve:
			AnimationUtility::splitCurve(mValue.getMinCurve(), mMinCurve);
			AnimationUtility::splitCurve(mValue.getMinCurve(), mMaxCurve);

			for(UINT32 i = 0; i < NumComponents; i++)
			{
				TCurveProperties<T>::setComponent(mMinConstant, i, mMinCurve[i].evaluate(0.0f));
				TCurveProperties<T>::setComponent(mMaxConstant, i, mMaxCurve[i].evaluate(0.0f));
			}

			break;
		case PDT_RandomCurveRange: 
			AnimationUtility::splitCurve(mValue.getMinCurve(), mMinCurve);
			AnimationUtility::splitCurve(mValue.getMaxCurve(), mMaxCurve);

			for(UINT32 i = 0; i < NumComponents; i++)
			{
				TCurveProperties<T>::setComponent(mMinConstant, i, mMinCurve[i].evaluate(0.0f));
				TCurveProperties<T>::setComponent(mMaxConstant, i, mMaxCurve[i].evaluate(0.0f));
			}

			break;
		}

		rebuild();
	}

	template<class T, class SELF>
	bool TGUIDistributionField<T, SELF>::hasInputFocus() const
	{
		if(mMinInput && mMinInput->hasInputFocus())
			return true;

		if(mMaxInput && mMaxInput->hasInputFocus())
			return true;

		return false;
	}

	template<class T, class SELF>
	void TGUIDistributionField<T, SELF>::setTint(const Color& color)
	{
		mDropDownButton->setTint(color);

		if (mLabel)
			mLabel->setTint(color);

		if(mMinInput)
			mMinInput->setTint(color);

		if(mMaxInput)
			mMaxInput->setTint(color);

		for(int i = 0; i < NumComponents; i++)
		{
			if(mCurveDisplay[i])
				mCurveDisplay[i]->setTint(color);
		}
	}

	template<class T, class SELF>
	Vector2I TGUIDistributionField<T, SELF>::_getOptimalSize() const
	{
		Vector2I optimalsize = mDropDownButton->_getOptimalSize();

		if(mLabel)
		{
			optimalsize.x += mLabel->_getOptimalSize().x;
			optimalsize.y = std::max(optimalsize.y, mLabel->_getOptimalSize().y);
		}

		if(impl::isVertical<T>())
		{
			if (mMinInput)
			{
				optimalsize.x = std::max(optimalsize.x, mMinInput->_getOptimalSize().x);
				optimalsize.y += mMinInput->_getOptimalSize().y;
			}

			if (mMaxInput)
			{
				optimalsize.x = std::max(optimalsize.x, mMaxInput->_getOptimalSize().x);
				optimalsize.y += mMaxInput->_getOptimalSize().y;
			}

			for(UINT32 i = 0; i < NumComponents; i++)
			{
				if (mCurveDisplay[i])
				{
					optimalsize.x = std::max(optimalsize.x, 50);
					optimalsize.y += mCurveDisplay[i]->_getOptimalSize().y;
				}
			}
		}
		else
		{
			if (mMinInput)
			{
				optimalsize.x += mMinInput->_getOptimalSize().x;
				optimalsize.y = std::max(optimalsize.y, mMinInput->_getOptimalSize().y);
			}

			if (mMaxInput)
			{
				optimalsize.x += mMaxInput->_getOptimalSize().x;
				optimalsize.y = std::max(optimalsize.y, mMaxInput->_getOptimalSize().y);
			}

			for(UINT32 i = 0; i < NumComponents; i++)
			{
				if (mCurveDisplay[i])
				{
					optimalsize.x += mCurveDisplay[i]->_getOptimalSize().x;
					optimalsize.y = std::max(optimalsize.y, 50);
				}
			}
		}

		return optimalsize;
	}

	template<class T, class SELF>
	void TGUIDistributionField<T, SELF>::styleUpdated()
	{
		mDropDownButton->setStyle(getSubStyleName(DROP_DOWN_FIELD_STYLE_TYPE));

		if (mLabel)
			mLabel->setStyle(getSubStyleName(getLabelStyleType()));

		if (mMinInput)
			mMinInput->setStyle(getSubStyleName(FLOAT_FIELD_STYLE_TYPE));

		if (mMaxInput)
			mMaxInput->setStyle(getSubStyleName(FLOAT_FIELD_STYLE_TYPE));

		for(int i = 0; i < NumComponents; i++)
		{
			if (mCurveDisplay[i])
				mCurveDisplay[i]->setStyle(getSubStyleName(CURVES_FIELD_STYLE_TYPE));
		}
	}

	template<class T, class SELF>
	void TGUIDistributionField<T, SELF>::rebuild()
	{
		if(mLabel)
			mLayout->removeElement(mLabel);

		mLayout->clear();
		mLayout->addElement(mLabel);

		GUILayout* valueLayout;
		
		if(impl::isVertical<T>())
			valueLayout = mLayout->addNewElement<GUILayoutY>();
		else
			valueLayout = mLayout;

		switch (mValue.getType())
		{
		default:
		case PDT_Constant:
			mMinInput = GUIConstantType::create(HString("Constant"), 50, GUIOptions(), getSubStyleName(FLOAT_FIELD_STYLE_TYPE));
			mMaxInput = nullptr;

			for(int i = 0; i < NumComponents; i++)
				mCurveDisplay[i] = nullptr;

			mMinInput->setValue(mMinConstant);
			mMinInput->onValueChanged.connect([this](T value)
			{
				mMinConstant = value;
				mValue = TDistribution<T>(value);

				onConstantModified();
			});
			mMinInput->onConfirm.connect([this]() { onConstantConfirmed(); });

			valueLayout->addElement(mMinInput);
			break;
		case PDT_RandomRange: 
			mMinInput = GUIConstantType::create(HString("Min."), 40, GUIOptions(), getSubStyleName(FLOAT_FIELD_STYLE_TYPE));
			mMaxInput = GUIConstantType::create(HString("Max."), 40, GUIOptions(), getSubStyleName(FLOAT_FIELD_STYLE_TYPE));

			for(int i = 0; i < NumComponents; i++)
				mCurveDisplay[i] = nullptr;

			mMinInput->setValue(mMinConstant);
			mMinInput->onValueChanged.connect([this](T value)
			{
				mMinConstant = value;
				mValue = TDistribution<T>(value, mMaxConstant);

				onConstantModified();
			});
			mMinInput->onConfirm.connect([this]() { onConstantConfirmed(); });

			mMaxInput->setValue(mMaxConstant);
			mMaxInput->onValueChanged.connect([this](T value)
			{
				mMaxConstant = value;
				mValue = TDistribution<T>(mMinConstant, value);

				onConstantModified();
			});
			mMaxInput->onConfirm.connect([this]() { onConstantConfirmed(); });

			valueLayout->addElement(mMinInput);
			valueLayout->addElement(mMaxInput);
			break;
		case PDT_Curve: 
			mMinInput = nullptr;
			mMaxInput = nullptr;

			for(int i = 0; i < NumComponents; i++)
			{
				mCurveDisplay[i] = GUICurves::create(CurveDrawOption::DrawMarkers, getSubStyleName(CURVES_FIELD_STYLE_TYPE));
				mCurveDisplay[i]->setCurves({ CurveDrawInfo(mMinCurve[i], Color::BansheeOrange) });

				mCurveDisplay[i]->setPadding(3);
				mCurveDisplay[i]->centerAndZoom();
				mCurveDisplay[i]->onClicked.connect([this,i]() { onClicked(i); });

				valueLayout->addElement(mCurveDisplay[i]);
			}

			break;
		case PDT_RandomCurveRange: 
			mMinInput = nullptr;
			mMaxInput = nullptr;

			for(int i = 0; i < NumComponents; i++)
			{
				mCurveDisplay[i] = GUICurves::create(CurveDrawOption::DrawMarkers | CurveDrawOption::DrawRange,
					getSubStyleName(CURVES_FIELD_STYLE_TYPE));

				mCurveDisplay[i]->setCurves(
					{
						CurveDrawInfo(mMinCurve[i], Color::BansheeOrange),
						CurveDrawInfo(mMaxCurve[i], Color::Red)
					});

				mCurveDisplay[i]->setPadding(3);
				mCurveDisplay[i]->centerAndZoom();
				mCurveDisplay[i]->onClicked.connect([this,i]() { onClicked(i); });

				valueLayout->addElement(mCurveDisplay[i]);
			}

			break;
		}

		mDropDownButton = GUIButton::create(HString::dummy(), getSubStyleName(DROP_DOWN_FIELD_STYLE_TYPE));
		mDropDownButton->onClick.connect([this]()
		{
			const Rect2I bounds = mDropDownButton->getBounds(mParentWidget->getPanel());
			const Vector2I center(bounds.x + bounds.width / 2, bounds.y + bounds.height / 2);

			mContextMenu->open(center, *mParentWidget);
		});

		mLayout->addNewElement<GUIFixedSpace>(10);
		mLayout->addElement(mDropDownButton);
	}

	template class BS_ED_EXPORT TGUIDistributionField<float, GUIFloatDistributionField>;
	template class BS_ED_EXPORT TGUIDistributionField<Vector2, GUIVector2DistributionField>;
	template class BS_ED_EXPORT TGUIDistributionField<Vector3, GUIVector3DistributionField>;

	const String& GUIFloatDistributionField::getGUITypeName()
	{
		static String typeName = "GUIFloatDistributionField";
		return typeName;
	}

	const String& GUIVector2DistributionField::getGUITypeName()
	{
		static String typeName = "GUIVector2DistributionField";
		return typeName;
	}

	const String& GUIVector3DistributionField::getGUITypeName()
	{
		static String typeName = "GUIVector3DistributionField";
		return typeName;
	}
}