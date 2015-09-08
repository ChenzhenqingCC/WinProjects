using UnityEngine;
using System.Collections;
using UnityEditor;
using System.IO;
using System.Diagnostics;

public class DataPacker : UnityEditor.EditorWindow {

	[MenuItem("Tools/DataPacker")]
	static void Create()
	{
		//EditorWindow.GetWindow (typeof(DataPacker));
		//ScriptableWizard.DisplayWizard("Create explosion");
		//Selection.objects
	}

	[MenuItem("Tools/XLS2CS")]
	public static void Execute3()
	{
		Object[] objects = Selection.objects;
		for (int iter = 0; iter < objects.Length; ++iter)
		{
			var dataPath = Application.dataPath;
			var filePah = dataPath + "/DataConfig/" + objects[iter].name + ".xlsx";
			if (File.Exists(filePah))
			{
				Process pro = new Process();
				var batPath = dataPath + "/../Configs/CSOnly.bat";
				FileInfo file = new FileInfo(batPath);
				pro.StartInfo.WorkingDirectory = file.Directory.FullName;
				pro.StartInfo.FileName = batPath;
				pro.StartInfo.Arguments = " " + objects[iter].name + " test";
				pro.StartInfo.CreateNoWindow = false;
				pro.Start();
				pro.WaitForExit();
			}
		}
	}

	[MenuItem("Tools/XLS2DATA")]
	public static void Execute4()
	{
		Object[] objects = Selection.objects;
		for (int iter = 0; iter < objects.Length; ++iter)
		{
			var dataPath = Application.dataPath;
			var filePah = dataPath + "/DataConfig/" + objects[iter].name + ".xlsx";
			if (File.Exists(filePah))
			{
				Process pro = new Process();
				var batPath = dataPath + "/../Configs/DataOnly.bat";
				FileInfo file = new FileInfo(batPath);
				pro.StartInfo.WorkingDirectory = file.Directory.FullName;
				pro.StartInfo.FileName = batPath;
				pro.StartInfo.Arguments = " " + objects[iter].name + " test";
				pro.StartInfo.CreateNoWindow = false;
				pro.Start();
				pro.WaitForExit();
			}
		}
	}
	
	void OnGUI()
	{
		//EditorGUILayout.LayerField();
	}
}
