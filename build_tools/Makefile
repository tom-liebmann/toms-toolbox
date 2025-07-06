LOCAL_PROJECT_DIR=${CURDIR}
DOCKER_PROJECT_DIR=/etc/project

LOCAL_CONAN_DIR=${HOME}/.conan2
DOCKER_CONAN_DIR=/etc/conan

CURRENT_USER_ID=$(shell id -u)
CURRENT_GROUP_ID=$(shell id -g)

BUILD_IMAGE=toms-toolbox/toms-toolbox_build:latest

.PHONY: build-docker-image
build-docker-image:
	DOCKER_BUILDKIT=0 docker build \
		-t ${BUILD_IMAGE} \
		-f ${LOCAL_PROJECT_DIR}/build_tools/Dockerfile \
		.

.PHONY: build
build:
	docker run \
		--user ${CURRENT_USER_ID}:${CURRENT_GROUP_ID} \
		--mount type=bind,source=${LOCAL_PROJECT_DIR},target=${DOCKER_PROJECT_DIR} \
		--mount type=bind,source=${LOCAL_CONAN_DIR},target=${DOCKER_CONAN_DIR} \
		"${BUILD_IMAGE}" \
		bash "${DOCKER_PROJECT_DIR}/build_tools/build.sh"
