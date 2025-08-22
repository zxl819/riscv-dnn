SIM := spike

# SUBDIRS= \
# 	add \
# 	avgpool \
# 	batchnorm \
# 	cast \
# 	conv \
# 	exp \
# 	matmul \
# 	maxpool \
# 	padding \
# 	relu \
# 	softmax

 SUBDIRS=matmul

all: $(SUBDIRS)

.PHONY:$(SUBDIRS)
$(SUBDIRS):
	@cd $@ && python test.py $(SIM) > test.log 2>&1
	@echo "$@ done."

