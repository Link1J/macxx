
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.IO;
using System.Text.Json;
using System.Text.Encodings.Web;

namespace meta.Json
{
	public class Serializer
	{
		public void Run(Reader reader, string outputFilename)
		{
			var options = new JsonSerializerOptions
			{
				Encoder = JavaScriptEncoder.UnsafeRelaxedJsonEscaping,
				WriteIndented = true,
				Converters =
				{
					new ReaderConverter(),
					new EnumConverter(),
					new ClassConverter(),
					new MethodConverter(),
					new PropertyConverter(),
					new FrameworkConverter(),
				}
			};
			var jsonString = JsonSerializer.Serialize(reader, options);
			File.WriteAllText(outputFilename, jsonString);
		}
	}
}
