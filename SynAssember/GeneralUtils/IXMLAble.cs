using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace GeneralUtils
{
    public interface IXMLAble
    {
        void write(XmlTextWriter writer);

        void read(XmlTextReader reader);

        bool hasChanged();
    }
}
