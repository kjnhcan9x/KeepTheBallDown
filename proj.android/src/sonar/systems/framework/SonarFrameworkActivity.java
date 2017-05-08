package sonar.systems.framework;

import org.cocos2dx.cpp.AppActivity;
import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Intent;
import android.os.Bundle;


public class SonarFrameworkActivity extends Cocos2dxActivity
{
	private static SonarFrameworkActivity app = null;
   
	public static void callShare() {
        Intent sharingIntent = new Intent(Intent.ACTION_SEND);
		sharingIntent.setType("text/plain");
		sharingIntent.putExtra(android.content.Intent.EXTRA_TEXT, "Check out Doodle Plane\nIt's an amazing game and you can get it for free at https://play.google.com/store/apps/details?id=com.glasses.doodleplane\nTry it now!");
		app.startActivity(Intent.createChooser(sharingIntent, "Share Game using"));
    }
	
	
	private static SonarFrameworkFunctions functions = null;
	
	protected SonarFrameworkActivity()
	{
		super();
	}
	
	@Override
	protected void onCreate(Bundle b) 
	{
		super.onCreate(b);
		try {
			functions = new SonarFrameworkFunctions(this);
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		functions.onCreate(b);
		app = this;
	}

	@Override
	protected void onStart() 
	{
		super.onStart();
		functions.onStart();
	}

	@Override
	protected void onStop() 
	{
		super.onStop();
		functions.onStop();
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		functions.onResume();
	}
	
	@Override
	protected void onActivityResult(int request, int response, Intent data)
	{
		super.onActivityResult(request, response, data);
		functions.onActivityResult(request, response, data);
	}
	
	@Override
	protected void onSaveInstanceState(Bundle outState) 
	{
	    super.onSaveInstanceState(outState);
	    functions.onSaveInstanceState(outState);
	}

	@Override
	public void onPause() 
	{
	    super.onPause();
	    functions.onPause();
	}

	@Override
	public void onDestroy() 
	{
	    super.onDestroy();
	    functions.onDestroy();
	}
	
	@Override
	public void onBackPressed() 
	{
			if(functions.onBackPressed())
				return;
			else
				super.onBackPressed();
	}
	//Cocos2d-x
	static 
	{
			System.loadLibrary("cocos2dcpp");
	}
}