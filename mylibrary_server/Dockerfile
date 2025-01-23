FROM gcc:latest

RUN apt-get update && apt-get install -y libpq-dev

WORKDIR /app

COPY . .

RUN gcc server.c controller.c richieste.c database.c -o server -pthread -I/usr/include/postgresql -lpq

CMD ["./server"]
