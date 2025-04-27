server: 
	g++ ../socket_kernels.cpp client.cpp -o client && \
	g++ ../socket_kernels.cpp Epoll.cpp server.cpp -o server

clean:
	rm server && rm client
