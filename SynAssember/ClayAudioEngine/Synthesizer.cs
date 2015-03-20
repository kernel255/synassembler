using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ClayAudioEngine
{
    class Synthesizer : IXMLAble
    {
        private List<AlgorithmGraph> m_Algorithms;

        public Synthesizer()
        {
            m_Algorithms = new List<AlgorithmGraph>();
        }

        public void addAlgorithm(AlgorithmGraph algo)
        {
            m_Algorithms.Add(algo);
        }

        public IList<AlgorithmGraph> getAlgorithms()
        {
            return m_Algorithms;
        }

        public void write(System.Xml.XmlTextWriter writer)
        {
            throw new NotImplementedException();
        }

        public void read(System.Xml.XmlTextReader reader)
        {
            throw new NotImplementedException();
        }

        public bool hasChanged()
        {
            throw new NotImplementedException();
        }

        public String getElementName()
        {
            return XML_ELEMENT_NAME;
        }

        private static readonly String XML_ELEMENT_NAME = "Synthesizer";
    }
}
