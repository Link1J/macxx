
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace meta.Json
{
	class EnumConverter : JsonConverter<Enum>
	{
		public override Enum Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
		{
			return null;
		}

		public override void Write(Utf8JsonWriter writer, Enum reader, JsonSerializerOptions options)
		{
			writer.WriteStartObject();
			writer.WriteString("name", reader.name);
			writer.WriteString("type", reader.type);
			writer.WriteBoolean("option", reader.option);

			writer.WriteStartObject("members");
			for (int a = 0; a < reader.members.Count; a++)
			{
				writer.WriteString(reader.members[a], reader.values[a]);
			}
			writer.WriteEndObject();
			writer.WriteEndObject();
		}
	}
}
