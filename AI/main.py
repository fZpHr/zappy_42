import argparse
import socket
import sys
import threading
import time

def client_session(host, port, team_name, session_id):
	try:
		client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		client.connect((host, port))
		
		welcome = client.recv(1024).decode().strip()
		print(f"Session {session_id} - Welcome: {welcome}")
		
		client.send((team_name + "\n").encode())
		
		nb_clients = client.recv(1024).decode().strip()
		
		if nb_clients == "Team full":
			print(f"Session {session_id} - Team {team_name} is full")
			client.close()
			return False
		
		print(f"Session {session_id} - Connected to team {team_name}, slots remaining: {nb_clients}")
		
		dimensions = client.recv(1024).decode().strip()
		print(f"Session {session_id} - Map dimensions: {dimensions}")
		
		while True:
			time.sleep(1)
			
	except Exception as e:
		print(f"Session {session_id} - Error: {e}")
		if 'client' in locals():
			client.close()
		return False

	return True

def try_connect_teams(host, port, team_names):
	session_count = 0
	threads = []

	while True:
		connected = False
		
		for team in team_names:
			thread = threading.Thread(target=client_session,args=(host, port, team, session_count),daemon=True)
			thread.start()
			threads.append(thread)
			
			time.sleep(0.5)
			
			if thread.is_alive():
				connected = True
				session_count += 1
				print(f"Successfully started session {session_count - 1} for team {team}")
				break
		
		if not connected:
			print("All teams are full, stopped creating new connections")
			break
		
		time.sleep(0.2)

	return threads

def main():
	parser = argparse.ArgumentParser(prog="python main.py", description="AI client will connect to the server and perform action", epilog="")
	parser.add_argument("--name", "-n", required=True, type=str, nargs='+', help="Name of the team")
	parser.add_argument("--port", "-p", required=True, type=int, help="Port of the server")
	parser.add_argument("--host", "-H", required=False, default="localhost", type=str, help="Ip of the server default localhost")
	args = parser.parse_args(sys.argv[1:])

	if args.port < 1024 or args.port > 65535:
		print("Wrong range of port")
		return 1

	print(f"Attempting to connect to server {args.host}:{args.port} with teams {args.name}")
	threads = try_connect_teams(args.host, args.port, args.name)

	try:
		print(f"Connected {len(threads)} clients. Press Ctrl+C to stop.")
		while any(thread.is_alive() for thread in threads):
			time.sleep(1)
	except KeyboardInterrupt:
		print("\nShutting down...")

if __name__ == "__main__":
	main()