STRUCT_DIR += red_black
STRUCT_DIR += skiplist

%/all:
	@echo "ENTER $(@D) (ALL $(PF_TARGET))"
	$(Q)$(MAKE) -C $(@D) all
	@echo "LEAVE $(@D)"

%/clean:
	@echo "ENTER $(@D) (CLEAN $(PF_TARGET))"
	$(Q)$(MAKE) -C $(@D) clean
	@echo "LEAVE $(@D)"



all: $(STRUCT_DIR:%=%/all)

clean: $(STRUCT_DIR:%=%/clean)


.PHONY: clean all
