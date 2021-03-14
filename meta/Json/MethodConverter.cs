
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace meta.Json
{
	class MethodConverter : JsonConverter<Method>
	{
		public override Method Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
		{
			return null;
		}

		public override void Write(Utf8JsonWriter writer, Method reader, JsonSerializerOptions options)
		{
			writer.WriteStartObject();
			writer.WriteString("name", reader.name);
			writer.WriteString("selector", reader.fullName);

			writer.WriteBoolean("static", reader.staticMethod);

			writer.WriteString("return", reader.returnType);

			writer.WriteStartArray("params");
			for (int a = 0; a < reader.paramType.Count; a++)
			{
				writer.WriteStartObject();
				writer.WriteString("type", reader.paramType[a]);
				writer.WriteString("name", reader.paramName[a]);
				writer.WriteEndObject();
			}
			writer.WriteEndArray();

			writer.WriteEndObject();
		}
	}
}
