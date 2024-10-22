all: up
up: setup
	sudo docker compose up kvirc
down:
	sudo docker compose down kvirc
setup:
	xhost + local:

list:
	docker ps -a
build:
	docker compose build

re: clean
	$(MAKE) all
clean:
	( \
		docker rm -f `docker ps -aq`; \
		docker image rm `docker image ls -aq` \
	) || true
