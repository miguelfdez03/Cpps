NAME    = scalar

CXX     = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS    = src/main.cpp \
          src/ScalarConverter.cpp

OBJDIR  = obj
OBJS    = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.o)))

INCLUDES = -I include

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
