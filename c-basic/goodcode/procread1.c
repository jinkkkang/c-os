//systemd-219/src/udev/net/link-config.c

static bool enable_name_policy(void) {
        _cleanup_free_ char *line = NULL;
        const char *word, *state;
        int r;
        size_t l;

        r = proc_cmdline(&line);
        if (r < 0) {
                log_warning_errno(r, "Failed to read /proc/cmdline, ignoring: %m");
                return true;
        }

        FOREACH_WORD_QUOTED(word, l, line, state)
                if (strneq(word, "net.ifnames=0", l))
                        return false;

        return true;
}