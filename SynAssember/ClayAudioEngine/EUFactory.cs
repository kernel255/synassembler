
using System.Collections.Generic;
using System;

namespace ClayAudioEngine
{

    public class ElaborationUnitFactory
    {
        private List<ElaborationUnitDescription> m_EUList;
        private int m_Id;
        public int Id
        {
            get { return m_Id; }
        }

        internal void addEU(ElaborationUnitDescription eu)
        {
            m_EUList.Add(eu);
        }

        internal ElaborationUnitFactory(String name, int id)
        {
            m_Name = name;
            m_Id = id;
            m_EUList = new List<ElaborationUnitDescription>();
        }

        public IList<ElaborationUnitDescription> getEUList()
        {
            return m_EUList;
        }

        public String getName()
        {
            return m_Name;
        }

        private String m_Name;
    }
}