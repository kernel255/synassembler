using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Drawing;
using SynthPanels;
using BasicAudioControls;

namespace BasicEUSynthFactory
{
    /// <summary>
    /// Interaction logic for OscillatorPanel.xaml
    /// </summary>
    public partial class OscillatorPanel : UserControl
    {
        SynthPanelManager.WriteEUDoubleProperty writeDProp;

        int id;

        public OscillatorPanel(int id, SynthPanelManager.WriteEUDoubleProperty writeDProp)
        {
            InitializeComponent();
            OutputLevel.setOwner(this);
            this.writeDProp = writeDProp;
            OutputLevel.SliderChangedEvent += new BasicSlider.SliderChanged(OutputLevel_SliderChangedEvent);
            this.id = id;
        }

        public static void OutputLevel_SliderChangedEvent(Object o, Double level)
        {
            OscillatorPanel oscPanel = (OscillatorPanel)o;
            oscPanel.writeDProp(oscPanel.id, "OutputLevel", level);
        }

        internal System.Drawing.Rectangle getRectangle()
        {
            return new System.Drawing.Rectangle(0, 0, 180, 220);
        }
    }
}
