NAME	= philo
CC		= gcc
CFLAGS	= -Werror -Wall -Wextra -pthread -g3

SRC_PATH = sources/
OBJ_PATH = objects/

SRC		=	main.c

SRCS	= $(addprefix $(SRC_PATH), $(SRC))
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))

INC		= -I ./includes/

all: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@
	@echo "✅ Philosophers compiled"

clean:
	@rm -rf $(OBJ_PATH)

fclean: clean
	@rm -f $(NAME)
	@echo "🧽 🪣 🧹 Philosophers cleaned"

re: fclean all

.PHONY: all re clean fclean