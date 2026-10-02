NAME        = btc

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98

SRCDIR      = src
INCDIR      = include
OBJDIR      = obj

SRCS        = $(SRCDIR)/main.cpp \
              $(SRCDIR)/BitcoinExchange.cpp

OBJS        = $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
