
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace meta.Json
{
	class FrameworkConverter : JsonConverter<Framework>
	{
		public override Framework Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
		{
			return null;
		}

		public override void Write(Utf8JsonWriter writer, Framework reader, JsonSerializerOptions options)
		{
			writer.WriteStartObject(reader.name);
			writer.WritePropertyName("enums");
			(options.GetConverter(reader.enums.GetType()) as JsonConverter<List<Enum>>).Write(writer, reader.enums, options);
			writer.WritePropertyName("classes");
			(options.GetConverter(reader.interfaces.GetType()) as JsonConverter<List<Class>>).Write(writer, reader.interfaces, options);
			writer.WriteEndObject();
		}
	}
}
