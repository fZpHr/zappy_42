using System;
using System.Text;
using System.Net.Sockets;
using System.Threading;
using UnityEngine;
using System.Collections.Generic;
using Newtonsoft.Json;

public class NetworkManager : MonoBehaviour
{
    [Header("Network Settings")]
    public string serverIP = "127.0.0.1";
    public int serverPort = 4244;
    
    [Header("Map Settings")]
    public GameObject[] tilePrefabs;
    public GameObject[] teamPrefabs;

    private TcpClient client;
    private NetworkStream stream;
    private Thread receiveThread;
    private string latestJsonData;

    private Queue<Action> mainThreadActions = new Queue<Action>();
    private MapData currentMapData;

    [System.Serializable]
    public class MapData
    {
        public int width;
        public int height;
        public List<List<string>> cells;
        public List<TeamData> teams;
    }

    [System.Serializable]
    public class TeamData
    {
        public string name;
        public List<int> position;
    }

    void Start()
    {
        ConnectToServer();
        InvokeRepeating("RequestMapUpdate", 1.0f, 1.0f);
    }

    void Update()
    {
        lock (mainThreadActions)
        {
            while (mainThreadActions.Count > 0)
            {
                mainThreadActions.Dequeue().Invoke();
            }
        }
    }

    void ConnectToServer()
    {
        try
        {
            client = new TcpClient();
            client.Connect(serverIP, serverPort);
            stream = client.GetStream();

            receiveThread = new Thread(ReceiveData);
            receiveThread.IsBackground = true;
            receiveThread.Start();
            Debug.Log("Connecté au serveur");
        }
        catch (Exception ex)
        {
            Debug.LogError("Erreur de connexion : " + ex.Message);
        }
    }

    void ReceiveData()
    {
        byte[] buffer = new byte[8192];
        try
        {
            while (client.Connected)
            {
                int bytesRead = stream.Read(buffer, 0, buffer.Length);
                if (bytesRead > 0)
                {
                    string jsonData = Encoding.UTF8.GetString(buffer, 0, bytesRead);
                    Debug.Log("JSON reçu : " + jsonData);
                    
                    lock (mainThreadActions)
                    {
                        mainThreadActions.Enqueue(() => ProcessMapData(jsonData));
                    }
                }
            }
        }
        catch (Exception ex)
        {
            Debug.LogError("Erreur réception : " + ex.Message);
        }
    }

    void ProcessMapData(string jsonData)
    {
        try
        {
            currentMapData = JsonConvert.DeserializeObject<MapData>(jsonData);
            // Debug.Log("Map cell : " + currentMapData.cells[1][13]);
            // Debug.Log("MapData traitée : " + currentMapData.width + "x" + currentMapData.height);
            ClearMap();
            Debug.Log("Map effacée");
            GenerateMap();
            Debug.Log("Map générée");
            // PlaceTeams();
            // Debug.Log("Équipes placées");
        }
        catch (Exception e)
        {
            Debug.LogError("Erreur traitement JSON : " + e.Message);
        }
    }

    void ClearMap()
    {
        foreach (Transform child in transform)
        {
            Destroy(child.gameObject);
        }
    }

    void GenerateMap()
    {
        for (int y = 0; y < currentMapData.height; y++)
        {
            Debug.Log($"Génération de la ligne {y}");
            for (int x = 0; x < currentMapData.width; x++)
            {
                Debug.Log($"Aled ({x}, {y})");
                Debug.Log($"currentMapData : {currentMapData.cells[y][x]}");
                string cell = currentMapData.cells[y][x];
                Debug.Log($"Génération de la cellule ({x}, {y}) : {cell}");
                InstantiateTile(cell, x, y);
                Debug.Log($"Cellule ({x}, {y}) générée");
            }
        }
    }

    void InstantiateTile(string cellType, int x, int y)
    {
        Debug.Log($"???: {cellType}");
        GameObject prefab = GetPrefabForCell(cellType);
        if (prefab == null)
        {
            Debug.LogError($"Prefab is null for cell type: {cellType}. Skipping instantiation.");
            return;
        }
        Debug.Log($"Prefab pour le type de cellule : {prefab}");
        if (prefab != null)
        {
            
            Debug.Log($"Instanciation du prefab pour le type de cellule : {cellType}");
            Vector3 position = new Vector3(x, 0, y); 
            Debug.Log($"Position de la cellule ({x}, {y}) : {position}");
            Instantiate(prefab, position, Quaternion.identity, transform);
        }
    }

    GameObject GetPrefabForCell(string cellType)
    {
        Debug.Log($"Type de cellule : {cellType}");
        switch (cellType)
        {
            case "0": 
                Debug.Log("CASE 0");
                return tilePrefabs[0];
            case "F":
                Debug.Log("CASE F");
                return tilePrefabs[1];
            case "1":
                Debug.Log("CASE 1");
                return tilePrefabs[2];
            case "2":
                Debug.Log("CASE 2");
                return tilePrefabs[3];
            default: 
                Debug.LogWarning($"Type de cellule inconnu: {cellType}");
                return null;
        }
    }

    void PlaceTeams()
    {
        foreach (TeamData team in currentMapData.teams)
        {
            if (team.position.Count >= 2)
            {
                int x = team.position[0];
                int y = team.position[1];
                Vector3 position = new Vector3(x, 0.5f, y); 
                Instantiate(teamPrefabs[0], position, Quaternion.identity);
            }
        }
    }

    void RequestMapUpdate()
    {
        if (client?.Connected == true)
        {
            byte[] request = Encoding.ASCII.GetBytes("MAP_REQUEST");
            stream.Write(request, 0, request.Length);
        }
    }

    void OnApplicationQuit()
    {
        receiveThread?.Abort();
        stream?.Close();
        client?.Close();
    }
}