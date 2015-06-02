using UnityEngine;
using System.Collections;

using System;
using System.IO;
using System.Net.Sockets;

public class TcpToEuler : MonoBehaviour {
	public Int32 Port = 8080;									// Port
	public string Host = "127.0.0.1";							// Host IP or domain.
	public string myText = "";									// Line received from the TCP stream
	public Vector3 currentRotation = new Vector3 (0, 0, 0);		// Line converted into a Vector3
	

	private bool socketReady = false;
	TcpClient mySocket;
	NetworkStream theStream;
	StreamWriter theWriter;
	StreamReader theReader;

	// Use this for initialization
	void Start () {

	}
	
	// Update is called once per frame used to receive TCP Socket-IO data.
	void Update () {
		string receivedText = readSocket();
		
		if (receivedText != "") {
			
			myText = receivedText;
			//print (myText);
			currentRotation = parseVector3(myText);
		}
	}


	// Basic connect and disconnect interface.
	void OnGUI(){	
		if (!socketReady) {  		
			if (GUI.Button (new Rect(0, 0, Screen.width /8, Screen.height/8), "Connect")) {			
				setupSocket ();			
			}		
		}

		if (socketReady) {  		
			if (GUI.Button (new Rect(0, 0, Screen.width /8, Screen.height/8), "Disconnect")) {			
				closeSocket();
			}		
		}
	}


	// Ensure that the socket closes on application close
	void OnApplicationQuit(){	
		closeSocket();		
	}

	// Opens the network TCP socket and starts read and write streams.
	public void setupSocket() {
		try {
			mySocket = new TcpClient(Host, Port);		
			theStream = mySocket.GetStream();		
			theWriter = new StreamWriter(theStream);	
			theReader = new StreamReader(theStream);

			socketReady = true;
		} catch (Exception e) {
			print(e.ToString());
		}
	}

	// Writes text through the stream over TCP
	public void writeSocket(string theLine){
		if (!socketReady) {
			return;
		}

		String foo = theLine + "\r\n";	
		theWriter.Write(foo);	
		theWriter.Flush();
	}

	// Reads a line of text from the TCP stream if available
	public String readSocket() {
		if (!socketReady) {
			return "";
		}

		if (theStream.DataAvailable) {
			return theReader.ReadLine();
		}
		return "";
	}

	// Closes TCP connection and streams.
	public void closeSocket() {
		if (!socketReady) {
			return;
		}

		theWriter.Close();
		theReader.Close();
		mySocket.Close();
		socketReady = false;
	}

	// Converts String "Value,Value,Value" into Vector3
	Vector3 parseVector3(string source) {
		Vector3 outVector3;
		String[] splitString = new String[3];

		if (source == "nan,nan,nan") {
			print ("Error: Received nans");
			return currentRotation;
		}
		splitString = source.Split ("," [0]);

		outVector3.x = float.Parse (splitString [0]);
		outVector3.y = float.Parse (splitString [1]);
		outVector3.z = float.Parse (splitString [2]);

		return outVector3;
	}

}
