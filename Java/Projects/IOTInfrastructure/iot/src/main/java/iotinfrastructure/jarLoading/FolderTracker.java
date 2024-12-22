package iotinfrastructure.jarLoading;

import java.io.IOException;
import java.nio.file.*;
import java.util.function.Consumer;

import static java.nio.file.StandardWatchEventKinds.ENTRY_CREATE;

public class FolderTracker {
    private final WatchService watchService;
    private final Path path; //blank final
    private boolean isRunning = false;

    public FolderTracker(String path) throws IOException {
        this.path = Paths.get(path);
        watchService = FileSystems.getDefault().newWatchService();
    }

    // default constructor

    public void track(Consumer<String> action) {
        isRunning = true;
        try {
            path.register(watchService, ENTRY_CREATE);
        } catch (IOException e) {
            /* exception will be thrown if the path is invalid, being checked earlier in code */
            throw new RuntimeException(e);
        }
        new Thread(() -> {
            while (isRunning) {
                final WatchKey key = watchService.poll();
                if (null == key) continue;
                for (WatchEvent<?> event : key.pollEvents()) {
                    action.accept(event.context().toString());
                }
                key.reset();
            }
        }).start();
    }

    private void stop() {
        isRunning = false;
    }
}