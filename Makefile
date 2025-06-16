server:
	g++ client.cpp -o client && \
	g++ util.cpp cppserver.cpp Epoll.cpp -o server
clean:
	rm server && rm client