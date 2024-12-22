package iotinfrastructure;

import iotinfrastructure.designPatterns.Callback;
import iotinfrastructure.designPatterns.Publisher;
import iotinfrastructure.jarLoading.DynamicJarLoader;
import iotinfrastructure.jarLoading.FolderTracker;

import java.io.IOException;
import java.util.List;
import java.util.function.Consumer;

public class PNPService {
    private final Publisher<List<Class<?>>> publisher = new Publisher<>();
    private final FolderTracker folderMonitor;
    private final DynamicJarLoader jarLoader = new DynamicJarLoader();
    private final String jarPath;

    public PNPService(String trackPath) throws IOException {
        this.jarPath = trackPath;
        this.folderMonitor = new FolderTracker(trackPath);
    }

    public void register(Consumer<List<Class<?>>> consumer) {
        publisher.register(new Callback<>(consumer, null));
    }

    public void start() {
        Consumer<String> action = path -> {
            List<Class<?>> classList = null;
            try {
                if (path.endsWith(".jar")) {
                    classList = jarLoader.loadClass("Command", jarPath + "/" + path);
                    publisher.publish(classList);
                }
            } catch (ClassNotFoundException | NoClassDefFoundError e) {
                System.out.println("Class not found " + e.getMessage());
            } catch (IOException e) {
                System.out.println("Jar file is empty, no class found");
            }
        };
        folderMonitor.track(action);
    }
}
