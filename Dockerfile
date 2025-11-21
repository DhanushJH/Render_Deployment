FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
g++ \
libssl-dev \
libcurl4-openssl-dev

WORKDIR /app

COPY . .

RUN g++ Day5.cpp -o server -lcurl -lssl -lcrypto -lpthread

EXPOSE 8080

CMD ["./server"]