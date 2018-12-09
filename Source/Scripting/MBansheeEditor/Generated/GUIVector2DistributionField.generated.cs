using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using BansheeEngine;

namespace BansheeEditor
{
	/** @addtogroup GUIEditor
	 *  @{
	 */

	/// <summary>
	/// A composite GUI object representing an editor field. Editor fields are a combination of a label and an input field. 
	/// Label is optional. This specific implementation displays an input field for a 2D vector distribution.
	/// </summary>
	[ShowInInspector]
	public partial class GUIVector2DistributionField : GUIElement
	{
		private GUIVector2DistributionField(bool __dummy0) { }
		protected GUIVector2DistributionField() { }

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelContent">Content to display in the editor field label.</param>
		/// <param name="labelWidth">Width of the label in pixels.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIVector2DistributionField(GUIContent labelContent, uint labelWidth, string style = "")
		{
			Internal_create(this, ref labelContent, labelWidth, style);
		}

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelContent">Content to display in the editor field label.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIVector2DistributionField(GUIContent labelContent, string style = "")
		{
			Internal_create0(this, ref labelContent, style);
		}

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelText">String to display in the editor field label.</param>
		/// <param name="labelWidth">Width of the label in pixels.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIVector2DistributionField(LocString labelText, uint labelWidth, string style = "")
		{
			Internal_create1(this, labelText, labelWidth, style);
		}

		/// <summary>Creates a new GUI editor field with a label.</summary>
		/// <param name="labelText">String to display in the editor field label.</param>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIVector2DistributionField(LocString labelText, string style = "")
		{
			Internal_create2(this, labelText, style);
		}

		/// <summary>Creates a new GUI editor field without a label.</summary>
		/// <param name="style">
		/// Optional style to use for the element. Style will be retrieved from GUISkin of the GUIWidget the element is used on. 
		/// If not specified default style is used.
		/// </param>
		public GUIVector2DistributionField(string style = "")
		{
			Internal_create3(this, style);
		}

		/// <summary>Changes the value of the field.</summary>
		[ShowInInspector]
		public Vector2Distribution Value
		{
			get { return Internal_getValue(mCachedPtr); }
			set { Internal_setValue(mCachedPtr, value); }
		}

		/// <summary>
		/// Checks if any of the float input fields currently have input focus. Only relevant for non-curve distributions.
		/// </summary>
		[ShowInInspector]
		public bool HasInputFocus
		{
			get { return Internal_hasInputFocus(mCachedPtr); }
		}

		/// <summary>
		/// Triggered when the user clicks on the curve display. Only relevant if the distribution is a curve distribution.  
		/// Provides the sequential index of the clicked curve (0 - x, 1 - y, 2 - z).
		/// </summary>
		partial void OnClicked(int p0);

		/// <summary>
		/// Triggered when the user modifies either of the non-curve (constant) values of the distribution. Only relevant if the 
		/// distribution is not a curve distribution.
		/// </summary>
		partial void OnConstantModified();

		/// <summary>
		/// Triggered when the user confirms inputs in either of the non-curve (constant) values of the distribution. Only  
		/// relevant if the distribution is not a curve distribution.
		/// </summary>
		partial void OnConstantConfirmed();

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern Vector2Distribution Internal_getValue(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_setValue(IntPtr thisPtr, Vector2Distribution value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_hasInputFocus(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create(GUIVector2DistributionField managedInstance, ref GUIContent labelContent, uint labelWidth, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create0(GUIVector2DistributionField managedInstance, ref GUIContent labelContent, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create1(GUIVector2DistributionField managedInstance, LocString labelText, uint labelWidth, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create2(GUIVector2DistributionField managedInstance, LocString labelText, string style);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_create3(GUIVector2DistributionField managedInstance, string style);
		private void Internal_onClicked(int p0)
		{
			OnClicked(p0);
		}
		private void Internal_onConstantModified()
		{
			OnConstantModified();
		}
		private void Internal_onConstantConfirmed()
		{
			OnConstantConfirmed();
		}
	}

	/** @} */
}
