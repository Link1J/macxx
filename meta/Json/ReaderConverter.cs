
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace meta.Json
{
	class ReaderConverter : JsonConverter<Reader>
	{
		public override Reader Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
		{
			return null;
		}

		public override void Write(Utf8JsonWriter writer, Reader reader, JsonSerializerOptions options)
		{
			writer.WriteStartObject();
			var converter = options.GetConverter(reader.frameworks[0].GetType()) as JsonConverter<Framework>;
			foreach (var item in reader.frameworks)
				converter.Write(writer, item, options);
			writer.WriteEndObject();
		}
	}
}
