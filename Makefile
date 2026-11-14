EXERCISES = ex00 ex01 ex02
ROOT_PROGRAMS = btc RPN PmergeMe

.PHONY: all clean fclean re

all:
	@for dir in $(EXERCISES); do $(MAKE) -C $$dir all || exit $$?; done
	cp ex00/btc ./btc
	cp ex01/RPN ./RPN
	cp ex02/PmergeMe ./PmergeMe

clean:
	@for dir in $(EXERCISES); do $(MAKE) -C $$dir clean || exit $$?; done

fclean:
	@for dir in $(EXERCISES); do $(MAKE) -C $$dir fclean || exit $$?; done
	rm -f $(ROOT_PROGRAMS)

re:
	@for dir in $(EXERCISES); do $(MAKE) -C $$dir re || exit $$?; done
	cp ex00/btc ./btc
	cp ex01/RPN ./RPN
	cp ex02/PmergeMe ./PmergeMe