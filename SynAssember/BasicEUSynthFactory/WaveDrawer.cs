using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Shapes;

namespace BasicEUFact
{
	class WaveManager
	{
		String filename;

		public byte[] riffID;		// "riff"
		public uint size;		
		public byte[] wavID;		// "WAVE"
		public byte[] fmtID;		// "fmt "
		public uint fmtSize;		// fmtチャンクのバイト数
		public ushort format;		// フォーマット
		public ushort channels;		// チャンネル数
		public uint sampleRate;		// サンプリングレート
		public uint bytePerSec;		// データ速度
		public ushort blockSize;	// ブロックサイズ
		public ushort bit;			// 量子化ビット数
		public byte[] dataID;		// "data"
		public uint dataSize;


		public WaveManager(String filename)
		{
			this.filename = filename;
		}

		static String RIFF = "RIFF";
		static String WAVE = "WAVE";

		bool compareBytes(byte[] read, String val)
		{
			System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
			String str = enc.GetString(read);
			return str.Equals(val);
		}

		bool isRIFF(byte[] read)
		{
			return compareBytes(read, RIFF);
 		}

		bool isWave(byte[] read)
		{
			return compareBytes(read, WAVE);
		}

		internal void DrawWave(Canvas canvas, Rect rect)
		{
			//byte[] wavContent = File.ReadAllBytes(filename);
			using (FileStream fs = new FileStream(filename, FileMode.Open, FileAccess.Read))
			{
				using (BinaryReader br = new BinaryReader(fs))
				{
					try
					{
						List<short> lDataList = new List<short>();
						List<short> rDataList = new List<short>();
						riffID = br.ReadBytes(4);
						if (!isRIFF(riffID))
							throw new Exception("Invalid WAV file. RIFF not available");
						
						size = br.ReadUInt32();
						wavID = br.ReadBytes(4);
						if (!isWave(wavID))
							throw new Exception("Invalid WAV file. WAVE not available");
						fmtID = br.ReadBytes(4);
						fmtSize = br.ReadUInt32();
						format = br.ReadUInt16();
						if (format != 1)
							throw new Exception("Invalid WAV file. Only linear quantization available.");
						channels = br.ReadUInt16();
						if (channels > 2)
							throw new Exception("Invalid WAV file. Maximum supported channels is 2.");
						sampleRate = br.ReadUInt32();
						bytePerSec = br.ReadUInt32();
						blockSize = br.ReadUInt16();
						bit = br.ReadUInt16();
						if (bit != 8 || bit != 16)
							throw new Exception("Invalid WAV file. 8 or 16 supported only");
						dataID = br.ReadBytes(4);
						dataSize = br.ReadUInt32();



						drawWave(canvas, rect, br);




						for (int i = 0; i < dataSize / blockSize; i++)
						{
							lDataList.Add((short)br.ReadUInt16());
							rDataList.Add((short)br.ReadUInt16());
						}
					}
					finally
					{
						if (br != null)
							br.Close();
						if (fs != null)
							fs.Close();
					}
				}
			}
		}

		internal void drawWave(Canvas canvas, Rect rect, BinaryReader br)
		{
			double heigth = rect.Height;

			long numSamples = dataSize / (channels * (bit / 8));
			long width = (long)rect.Width;

			double step = numSamples / width;

			for(int i=0;i<width;i++)
			{
				Line line = new Line();

			}

		}
	}
}
