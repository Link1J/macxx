
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace meta.Json
{
	class ClassConverter : JsonConverter<Class>
	{
		public override Class Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
		{
			return null;
		}

		public override void Write(Utf8JsonWriter writer, Class reader, JsonSerializerOptions options)
		{
			writer.WriteStartObject();
			writer.WriteBoolean("protocol", reader.protocol);
			writer.WriteString("name", reader.name);
			writer.WriteString("baseClass", reader.baseClass);

			writer.WritePropertyName("protocols");
			(options.GetConverter(reader.protocols.GetType()) as JsonConverter<List<string>>).Write(writer, reader.protocols, options);

			writer.WritePropertyName("methods");
			(options.GetConverter(reader.methods.GetType()) as JsonConverter<List<Method>>).Write(writer, reader.methods, options);
			writer.WritePropertyName("properties");
			(options.GetConverter(reader.properties.GetType()) as JsonConverter<List<Property>>).Write(writer, reader.properties, options);

			writer.WriteEndObject();
		}
	}
}
