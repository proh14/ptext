CC=gcc
TARGET_EXEC := ptext
MANPAGE := ptext.1
BUILD_DIR := ./build
INSTALL_DIR := /usr/local/bin
MANPAGE_INSTALL_DIR := /usr/local/share/man/man1
SRC_DIRS := ./src
MANPAGE_DIR := ./docs/
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CFLAGS := $(INC_FLAGS) -MMD -MP -Wall -g -Wextra -pedantic -std=c99
LDFLAGS := 

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: install clean uninstall uninstall-manpage clean-all install-manpage man-compile

man-compile: $(MANPAGE_DIR)/$(MANPAGE)
	groff -Tpdf -man $(MANPAGE_DIR)/$(MANPAGE) > $(MANPAGE_DIR)/manual.pdf
	groff -Tascii -man $(MANPAGE_DIR)/$(MANPAGE) > $(MANPAGE_DIR)/manual.txt
	groff -Thtml -man $(MANPAGE_DIR)/$(MANPAGE) > $(MANPAGE_DIR)/manual.html
	pandoc -f man -t markdown $(MANPAGE_DIR)/$(MANPAGE) > $(MANPAGE_DIR)/manual.md


install: $(BUILD_DIR)/$(TARGET_EXEC) install-manpage
	install -m 755 $< $(INSTALL_DIR)/$(TARGET_EXEC)

install-manpage: $(MANPAGE_DIR)/$(MANPAGE)
	mkdir -p $(MANPAGE_INSTALL_DIR)
	install -D -m 644 $(MANPAGE_DIR)/$(MANPAGE) $(MANPAGE_INSTALL_DIR)/$(MANPAGE)

uninstall: $(INSTALL_DIR)/$(TARGET_EXEC)
	rm $(INSTALL_DIR)/$(TARGET_EXEC) 

clean:
	rm -r $(BUILD_DIR)

uninstall-manpage: $(MANPAGE_INSTALL_DIR)/$(MANPAGE)
	rm $(MANPAGE_INSTALL_DIR)/$(MANPAGE)

clean-all: uninstall uninstall-manpage clean

-include $(DEPS)
