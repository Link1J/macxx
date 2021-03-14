
// Copyright Jared Irwin 2021
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

using System;
using System.IO;
using System.Collections.Generic;
using System.Runtime.InteropServices;
// using System.Reflection;
// using System.Reflection.Metadata.Ecma335;
// using System.Reflection.Metadata;
// using System.Reflection.PortableExecutable;
using System.Text;

namespace meta
{
	/*
	class Program
	{
		static void Main(string[] args)
		{
			var macosSDK = "R:/MacOS/MacOS SDK/MacOSX10.15.sdk";

			var translationFlags = CXTranslationUnit_Flags.CXTranslationUnit_None;
			translationFlags |= CXTranslationUnit_Flags.CXTranslationUnit_SkipFunctionBodies;
			translationFlags |= CXTranslationUnit_Flags.CXTranslationUnit_IncludeAttributedTypes;
			translationFlags |= CXTranslationUnit_Flags.CXTranslationUnit_VisitImplicitAttributes;
			var clangCommandLineArgs = new string[]{
				"-Wall", //"-Wno-pragma-once-outside-header",
				"-x", "objective-c",
				"-v",
				"-fblocks",
				"-target", "x86_64-apple-macosx10.15",
				"-isysroot" + macosSDK,
				"-isystem" + "C:/Program Files/LLVM/lib/clang/11.0.1/include",
				};

			//foreach (var framework in Directory.GetDirectories(macosSDK + @"\System\Library\Frameworks"))
			{
				//foreach (var filePath in Directory.GetFiles(framework + @"\Headers", "*.h"))
				{
					var filePath = macosSDK + "/System/Library/Frameworks/Foundation.framework/Headers/NSObject.h";

					var IndexHandle = CXIndex.Create(false, true);
					var unitError = CXTranslationUnit.TryParse(IndexHandle, filePath, clangCommandLineArgs, Array.Empty<CXUnsavedFile>(), translationFlags, out CXTranslationUnit handle);
					var skipProcessing = false;

					if (unitError != CXErrorCode.CXError_Success)
					{
						Console.WriteLine($"Error: Parsing failed for '{filePath}' due to '{unitError}'.");
						skipProcessing = true;
					}
					else if (handle.NumDiagnostics != 0)
					{
						Console.WriteLine($"Diagnostics for '{filePath}':");

						for (uint i = 0; i < handle.NumDiagnostics; ++i)
						{
							using var diagnostic = handle.GetDiagnostic(i);

							Console.Write("    ");
							Console.WriteLine(diagnostic.Format(CXDiagnostic.DefaultDisplayOptions).ToString());

							skipProcessing |= (diagnostic.Severity == CXDiagnosticSeverity.CXDiagnostic_Error);
							skipProcessing |= (diagnostic.Severity == CXDiagnosticSeverity.CXDiagnostic_Fatal);
						}
					}

					if (skipProcessing)
					{
						Console.WriteLine($"Skipping '{filePath}' due to one or more errors listed above.");
						Console.WriteLine();
						return; //continue;
					}

					unsafe
					{
						handle.Cursor.VisitChildren((cursor, parent, clientData) =>
						{
							if (!cursor.Location.IsFromMainFile)
								return CXChildVisitResult.CXChildVisit_Continue;

							if (cursor.Kind == CXCursorKind.CXCursor_ObjCProtocolDecl)
							{
								Type.NewProtocol(cursor.DisplayName.CString);
							}
							else if (cursor.Kind == CXCursorKind.CXCursor_ObjCInstanceMethodDecl)
							{
								var method = Method.New(cursor.DisplayName.CString);
								{
									Type type = Type.get(parent.DisplayName.CString);
									type.addMethod(method);
								}
							}
							else if (cursor.Kind == CXCursorKind.CXCursor_ObjCClassMethodDecl)
							{
								var method = Selector.New(cursor.DisplayName.CString);
								try
								{
									Type type = Type.get(parent.DisplayName.CString);
									type.addMethod(method);
								}
								catch (Exception e)
								{

								}
							}
							else if (cursor.Kind == CXCursorKind.CXCursor_TypeRef)
							{
								if (parent.Kind == CXCursorKind.CXCursor_ObjCInstanceMethodDecl)
								{
									var method = Method.get(parent.DisplayName.CString);
									method.addReturnType(cursor.DisplayName.CString);
								}
								else if (parent.Kind == CXCursorKind.CXCursor_ParmDecl)
								{
									var method = Method.get(parent.ParentFunctionOrMethod.DisplayName.CString);
									method.addParameterType(cursor.DisplayName.CString);
								}
							}
							else if (cursor.Kind == CXCursorKind.CXCursor_ParmDecl)
							{
								var method = Method.get(parent.DisplayName.CString);
								method.addParameter(cursor.DisplayName.CString);
							}
							else if (cursor.Kind == CXCursorKind.CXCursor_ObjCPropertyDecl)
							{
								var temp1 = parent.SemanticParent;
								var temp2 = parent.ParentFunctionOrMethod;
								var temp3 = parent.LexicalParent;

								var property = Property.New(cursor.DisplayName.CString, parent.DisplayName.CString);
								var type = Type.get(parent.DisplayName.CString);
							}
							else if (cursor.Kind == CXCursorKind.CXCursor_ObjCCategoryDecl)
							{
							}
							else
							{
								Console.WriteLine($"{cursor.Kind} | {cursor}");
							}

							return CXChildVisitResult.CXChildVisit_Recurse;
						}, clientData: default);
					}
				}
			}

			var assemblyName = "macOS.dll";

			MetadataBuilder metadataBuilder = new MetadataBuilder();
			metadataBuilder.AddAssembly(metadataBuilder.GetOrAddString(assemblyName), new Version(1, 0, 0, 0), default, default, default, AssemblyHashAlgorithm.None);

			var moduleRef = metadataBuilder.AddModule(0, metadataBuilder.GetOrAddString(assemblyName), metadataBuilder.GetOrAddGuid(Guid.NewGuid()), default, default);

			metadataBuilder.AddTypeDefinition(default, default, metadataBuilder.GetOrAddString("<Module>"), baseType: default(EntityHandle), fieldList: MetadataTokens.FieldDefinitionHandle(1), methodList: MetadataTokens.MethodDefinitionHandle(1));


			var peBuilder = new ManagedPEBuilder(PEHeaderBuilder.CreateLibraryHeader(), new MetadataRootBuilder(metadataBuilder), ilStream: new BlobBuilder());

			var peBlob = new BlobBuilder();
			peBuilder.Serialize(peBlob);

			using (FileStream fileStream = new FileStream(assemblyName, FileMode.Create, FileAccess.ReadWrite))
			{
				peBlob.WriteContentTo(fileStream);
			}
		}
	}*/

	enum Mode
	{
		Global,
		Preprocessor,
		ObjectiveC,
		ObjC_Refs,
		ObjC_Class,
		ObjC_Implementation,
		Block,
		ObjC_Protocol,
		ObjC_Details,
		ObjC_Method,
	}

	class Program
	{
		static void Main(string[] args)
		{
			var macosSDK = "R:/MacOS/MacOS SDK/MacOSX10.15.sdk";

			var reader = new Reader();
			reader.Run(new Parser(macosSDK + "/System/Library/Frameworks/Foundation.framework/Headers/NSObject.h"));
			reader.Run(new Parser(macosSDK + "/System/Library/Frameworks/Foundation.framework/Headers/NSString.h"));

			Console.WriteLine($"Enums:");
			reader.enums.ForEach(x =>
			{
				Console.WriteLine($"\t{x.name}");
				x.members.ForEach(y =>
				{
					Console.WriteLine($"\t\t{y}");
				});
			});
			Console.WriteLine($"Protocols:");
			reader.protocols.ForEach(x =>
			{
				Console.WriteLine($"\t{x.name}");
				Console.WriteLine($"\t\tMethods:");
				x.methods.ForEach(y =>
				{
					Console.WriteLine($"\t\t\t{y.fullName}, {y.staticMethod}");
				});
				Console.WriteLine($"\t\tProperties:");
				x.properties.ForEach(y =>
				{
					Console.WriteLine($"\t\t\t{y.name}, {y.type}");
				});
			});
			Console.WriteLine($"Interfaces:");
			reader.interfaces.ForEach(x =>
			{
				var baseClass = x.baseClass != null ? ": " + x.baseClass : "";
				Console.WriteLine($"\t{x.name} {baseClass}");
				Console.WriteLine($"\t\tMethods:");
				x.methods.ForEach(y =>
				{
					Console.WriteLine($"\t\t\t{y.fullName}, {y.staticMethod}");
				});
				Console.WriteLine($"\t\tProperties:");
				x.properties.ForEach(y =>
				{
					Console.WriteLine($"\t\t\t{y.name}, {y.type}");
				});
			});
		}
	}
}
