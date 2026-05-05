NAME = abstract

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = src/main.cpp src/Animal.cpp src/Dog.cpp src/Cat.cpp src/WrongAnimal.cpp src/WrongCat.cpp src/Brain.cpp
OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.o)))

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re