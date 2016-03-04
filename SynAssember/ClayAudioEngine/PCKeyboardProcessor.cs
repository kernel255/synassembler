using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;

namespace ClayAudioEngine
{
	class PCKeyboardProcessor
	{
		Dictionary<Key, bool> activeKeyMap = new Dictionary<Key, bool>();

		public void Reset()
		{
			activeKeyMap.Clear();
		}

		public byte[] ProcessPCKeyboardEvent(KeyEventArgs e)
		{
			if (e.IsRepeat)
				return null;
			byte[] result = TranslateKeyToMIDIMsg(e);
			if (result != null)
			{
				if (e.IsDown)
				{
					if (activeKeyMap.ContainsKey(e.Key))
					{
						if (!activeKeyMap[e.Key])
						{
							activeKeyMap[e.Key] = true;
						}
						else
							return null;
					}
					else
						activeKeyMap[e.Key] = true;
				}
				if (e.IsUp)
				{
					if (activeKeyMap.ContainsKey(e.Key))
					{
						activeKeyMap[e.Key] = false;
					}
				}
			}
			return result;
		}

		byte GetMIDINoteByKey(Key key)
		{
			switch (key)
			{
				case Key.Q:
					return 48;
				case Key.D2:
					return 49;
				case Key.W:
					return 50;
				case Key.D3:
					return 51;
				case Key.E:
					return 52;
				case Key.R:
					return 53;
				case Key.D5:
					return 54;
				case Key.T:
					return 55;
				case Key.D6:
					return 56;
				case Key.Y:
					return 57;
				case Key.D7:
					return 58;
				case Key.U:
					return 59;
				case Key.I:
					return 60;
				default:
					return 0xFF;
			}
		}

		const byte MIDI_CHANNEL = 0x00;
		const byte MIDI_NOTE_ON = 0x90;
		const byte MIDI_NOTE_OFF = 0x80;
		const byte MIDI_DEFAULT_VELOCITY = 0x7F;

		byte[] TranslateKeyToMIDIMsg(KeyEventArgs e)
		{
			byte[] result = new byte[3];
			byte note = GetMIDINoteByKey(e.Key);
			if (note == 0xFF)
				return null;
			if (e.IsDown)
				result[0] = MIDI_NOTE_ON | MIDI_CHANNEL;
			if (e.IsUp)
				result[0] = MIDI_NOTE_OFF | MIDI_CHANNEL;
			if (!e.IsDown && !e.IsUp)
				return null;

			result[1] = note;
			result[2] = MIDI_DEFAULT_VELOCITY;

			return result;
		}

		int euId = -1;
		internal int EUId
		{
			set
			{
				euId = value;
			}
			get
			{
				return euId;
			}
		}
			

	}
}
