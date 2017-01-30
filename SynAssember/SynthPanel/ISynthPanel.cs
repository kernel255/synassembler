using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using System.Drawing;
using GeneralUtils;

namespace SynthPanels
{
    public interface ISynthPanel : IXMLAble
    {
        UserControl getUserControlPanel();
        Rectangle getRect();
		void readParametersFromEngine();
		int GetHorizontalSpan();
		int GetVerticalSpan();
		/**
		 * Absolute position in container elements coordinates
		 */
		int x
		{
			get;
			set;
		}
		int y
		{
			get;
			set;
		}
		/**
		 * Position mapped in SynthLayoutManager (integers)
		 */
		int i
		{
			get;
			set;
		}
		int j
		{
			get;
			set;
		}
	}

}
