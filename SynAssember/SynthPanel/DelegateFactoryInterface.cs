using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SynthPanels
{
	public delegate int WriteEUProperty(int id, int propertyIndex, String value);
	public delegate String ReadEUProperty(int id, int propertyIndex);
	// Double
	public delegate int WriteEUDoubleProperty(int id, int propertyIndex, Double val);
	public delegate double ReadEUDoubleProperty(int id, int propertyIndex);
	// Integer
	public delegate int WriteEUIntegerProperty(int id, int propertyIndex, Int32 val);
	public delegate int ReadEUIntegerProperty(int id, int propertyIndex);
	// Boolean
	public delegate int WriteEUBoolProperty(int id, int propertyIndex, bool val);
	public delegate bool ReadEUBoolProperty(int id, int propertyIndex);

	public interface DelegateFactoryInterface
	{
		WriteEUDoubleProperty getWriteEUDubleProperty();
	}
}
