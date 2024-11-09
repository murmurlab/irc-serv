CC				= c++

LXXFLAGS		= 
RUNFLAGS		=

EXE_DIR			= 
ROOT			= irc-serv/
NAME			= ircserv
TEST			= irctest
LIB				= $(ROOT)/lib
CMD_DIR			= $(ROOT)/cmd
SRC_DIR			= $(ROOT)/
INC_DIR			= $(ROOT)/inc
INCS			= $(wildcard $(INC_DIR)/*.tpp) $(wildcard $(INC_DIR)/*.hpp) $(wildcard $(INC_DIR)/*.h)
OBJ_DIR			= .
SRCS			= $(wildcard $(SRC_DIR)/*.cpp)
OBJS			= $(SRCS:%.cpp=%.o)
CMD_IRCSERV		= $(CMD_DIR)/$(NAME).o
CMD_IRCTEST		= $(CMD_DIR)/$(TEST).o
CMDS			= $(CMD_IRCSERV) $(CMD_IRCTEST)
DEPENDENCIES	=

CXXFLAGS        = -Wc99-designator -std=c++98 -pedantic -I $(INC_DIR)

os =  ${shell uname -s}
ifeq '$(os)' 'Darwin'
NPROCS  = $(shell sysctl -n hw.ncpu)
else ifeq '$(os)' 'Linux'
NPROCS  = $(shell nproc)
endif
# MAKEFLAGS += -j$(NPROCS)
gnu = 1
ifeq '$(gnu)' '1'
CXXFLAGS += $(stdcxx_cxxflags)
LXXFLAGS += $(stdcxx_lxxflags)
endif

w = 1
ifeq '$(w)' '1'
CXXFLAGS += -Wextra -Werror -Wall
endif
debug = 1
ifeq '$(debug)' '1'
CXXFLAGS += -g
endif
asan = 0
ifeq '$(asan)' '1'
RUNFLAGS += ASAN_OPTIONS='detect_leaks=1'
LXXFLAGS += -lasan #-fsanitize=address
# LXXFLAGS += -fsanitize=thread
endif
test = 0
ifeq '$(test)' '1'
./$(attest) .
endif

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@
all: $(DEPENDENCIES)
	@$(MAKE) $(NAME) #$(TEST)
$(NAME): $(OBJS) $(CMD_IRCSERV) $(INCS)
	$(CC) $(LXXFLAGS) $(CMD_IRCSERV) $(OBJS) -o $(NAME)
$(TEST): $(OBJS) $(CMD_IRCTEST) $(INCS)
	$(CC) $(LXXFLAGS) $(CMD_IRCTEST) $(OBJS) -o $(TEST)
run: all
	$(RUNFLAGS) ./$(NAME) $(args)
test: all
	$(RUNFLAGS) ./$(TEST) $(args)
down:
	lsof -c ircserv -sTCP:LISTEN | grep 'IPv4' |  awk '{print $$2}' | xargs -I @ kill -9 @
clean:
	$(RM) $(OBJS) $(B_OBJ) $(CMDS)
fclean: clean
	$(RM) $(NAME) $(TEST) & wait
re: fclean
	$(MAKE) all
.PHONY: all clean fclean re run
