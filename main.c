#include <fcntl.h>
#include <libnotify/notify.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FAN_NODE "/sys/devices/platform/asus-nb-wmi/throttle_thermal_policy"

enum fanMode {
	performance,
	turbo,
	silent
};
const char *messages[] = {
	[silent] = "Switched to silent mode.",
	[performance] = "Switched to performance mode.",
	[turbo] = "Switched to turbo mode.",
};
const char *icons[] = {
	[silent] = "sensors-fan-symbolic",
	[performance] = "sensors-fan-symbolic",
	[turbo] = "sensors-fan-symbolic",
};

void notify(NotifyNotification *notif, int fanMode) {
	notify_notification_update(notif, "Fan mode", messages[fanMode], icons[fanMode]);
	notify_notification_show(notif, NULL);
}

int main(int argc, char *argv[]) {
	int fd = open(FAN_NODE, O_RDONLY);
	if (fd < 0) {
		printf("Failed, sysfs node " FAN_NODE " not exported.\n");
		exit(1);
	}

	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLPRI;
	char fanMode;

	// Consume prior updates
	lseek(fd, 0, SEEK_SET);
	read(fd, &fanMode, sizeof(fanMode));

	notify_init("Fan Mode");
	NotifyNotification *notif = notify_notification_new(NULL, NULL, NULL);
	while (1) {
		// Wait for change
		poll(&pfd, 1, -1);

		// Read updated value
		lseek(fd, 0, SEEK_SET);
		read(fd, &fanMode, sizeof(fanMode));
		notify(notif, fanMode - '0');
	}
}
