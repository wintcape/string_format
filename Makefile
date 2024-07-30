## ( see build/Makefile to adjust build settings )
LINUX := linux
WINDOWS := windows

################################################################################

default:
	@echo Please choose from the available targets:    linux  windows
	@exit 2

################################################################################

.PHONY: linux
linux:
	@make -f build/$(LINUX).make app

################################################################################

.PHONY: windows
windows:
	@make -f build/$(WINDOWS).make app	

################################################################################