using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Shapes;
using System.Windows.Controls;

namespace ClayAudioEngine
{
    interface ICanavasable
    {
        /**
         * Link the ICanavasable to a Canvas
         */
        void addToCanvas(Canvas canvas);

        void moveByX(double x);
        void moveByY(double y);
    }
}
