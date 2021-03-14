
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace meta.Json
{
	class PropertyConverter : JsonConverter<Property>
	{
		public override Property Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
		{
			return null;
		}

		public override void Write(Utf8JsonWriter writer, Property reader, JsonSerializerOptions options)
		{
			writer.WriteStartObject();
			
			writer.WriteString("name", reader.name);
			writer.WriteString("type", reader.type);

			writer.WriteBoolean("static", reader.staticProperty);
			writer.WriteBoolean("read", reader.read);
			writer.WriteBoolean("write", reader.write);

			writer.WriteString("getter", reader.getter);
			writer.WriteString("setter", reader.setter);

			writer.WriteEndObject();
		}
	}
}
