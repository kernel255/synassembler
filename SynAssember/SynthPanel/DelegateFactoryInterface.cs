using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SynthPanels
{
	public delegate bool WriteEUProperty(Int32 id, String propertyName, String value);

	public delegate int WriteEUDoubleProperty(int id, int propertyIndex, Double val);

	public delegate int WriteEUIntegerProperty(int id, int propertyIndex, Int32 val);

	public interface DelegateFactoryInterface
	{
		WriteEUDoubleProperty getWriteEUDubleProperty();
	}
}
