INCLUDE_PATH=../include

CONTAINERS =list \
	xorlist \
	vector \
	deque \
	map \
	set \
	multimap \
	multiset \
	stack \
	queue \
	priority_queue

CPP11_CONTAINERS = \
	unordered_map \
	unordered_set \
	unordered_multimap \
	unordered_multiset \

OBJ_DIR=.objs

USER_CONTAINERS = $(foreach a, $(CONTAINERS) $(CPP11_CONTAINERS),ft_${a})

DEPS=$(foreach c, $(USER_CONTAINERS), ${OBJ_DIR}/$c.d)

STD_CONTAINERS = $(foreach a, $(CONTAINERS),std_${a})

STD_CPP11_CONTAINERS = $(foreach a, $(CPP11_CONTAINERS),std_${a})

to_upper=$(shell echo $(1) | tr '[a-z]' '[A-Z]')

remove_ns=$(shell echo $(1) | sed 's/[^_]*_//')

all: $(CONTAINERS) $(CPP11_CONTAINERS)

$(USER_CONTAINERS): ${OBJ_DIR}
	c++ srcs/main.cpp -D TEST_$(call to_upper, $(call remove_ns, $@)) -Wall -Wextra -Werror -MMD -MF ${OBJ_DIR}/$@.d -MT $@ -g3 -std=c++98 -I ${INCLUDE_PATH}  -D TEST_FT -o $@



$(STD_CONTAINERS):
	c++ srcs/main.cpp -D TEST_$(call to_upper, $(call remove_ns, $@)) -std=c++98 -D TEST_STD -o $(@)

$(STD_CPP11_CONTAINERS):
	c++ srcs/main.cpp -D TEST_$(call to_upper, $(call remove_ns, $@)) -std=c++11 -D TEST_STD -o $(@)

.SECONDEXPANSION:

$(CONTAINERS) $(CPP11_CONTAINERS): ft_$$(@) std_$$(@)

${OBJ_DIR}:
	mkdir $@

clean:
	rm -f $(DEPS)

fclean: clean
	rm -rf $(USER_CONTAINERS) $(STD_CONTAINERS) $(STD_CPP11_CONTAINERS) $(OBJ_DIR)

re: fclean all

-include $(DEPS)
