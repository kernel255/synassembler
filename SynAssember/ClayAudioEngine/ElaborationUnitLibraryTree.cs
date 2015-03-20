using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using ClayAudioEngine;

namespace SynAssember
{
    class ElaborationUnitLibraryTree
    {
        private IList<ElaborationUnitFactory> m_Factories;

        private TreeView m_TreeView;

        public ElaborationUnitLibraryTree(TreeView view, IList<ElaborationUnitFactory> factories)
        {
            m_Factories = factories;
            m_TreeView = view;
        }










        public void drawTree()
        {
            int numFactories = m_Factories.Count;
            for (int factoryIndex = 0; factoryIndex < numFactories; factoryIndex++)
            {
                ElaborationUnitFactory factory = m_Factories.ElementAt(factoryIndex);
                IList<ElaborationUnitDescription> euList = factory.getEUList();
                String factoryName = factory.getName();

                //TreeNode factoryNode = new TreeNode(factory.getName());

                TreeViewItem factItem = new TreeViewItem();
                factItem.Header = factory.getName();
                m_TreeView.Items.Add(factItem);

                int numEU = factory.getEUList().Count;
                for (int euIndex = 0; euIndex < numEU; euIndex++)
                {
                    ElaborationUnitDescription euDesc = factory.getEUList()[euIndex];
                    TreeViewItem euItem = new TreeViewItem();
                    euItem.Header = euDesc.Name;
                    factItem.Items.Add(euItem);

                    int numInstances = euDesc.getInstances().Count;
                    for (int instIndex = 0; instIndex < numInstances; instIndex++)
                    {
                        ElaborationUnitPhysicalInstance euInst = euDesc.getInstances()[instIndex];
                        TreeViewItem instItem = new TreeViewItem();
                        instItem.Header = euInst.m_Name;
                        euItem.Items.Add(instItem);
                    }
                    
                }

                

            }
        }

    }
}
