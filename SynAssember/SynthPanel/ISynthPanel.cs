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
        int GetX();
        int GetY();
		void readParametersFromEngine();
		int GetHorizontalSpan();
		int GetVerticalSpan();
	}
   
}
