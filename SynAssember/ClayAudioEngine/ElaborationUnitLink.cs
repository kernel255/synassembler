using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using ClayAudioEngine;
using EUShelves;


namespace SynAssembler
{
    class ElaborationUnitLink
    {
        public ElaborationUnitLink(ElaborationUnitGlyphInstance inEU, Port inPort, ElaborationUnitGlyphInstance outEU, Port outPort)
        {
            m_InputEU = inEU;
            m_OutputEU = outEU;
            m_InPort = inPort;
            m_OutPort = outPort;
        }

        public ElaborationUnitGlyphInstance m_InputEU;
        public ElaborationUnitGlyphInstance m_OutputEU;
        public Port m_InPort;
        public Port m_OutPort;

    }
}
