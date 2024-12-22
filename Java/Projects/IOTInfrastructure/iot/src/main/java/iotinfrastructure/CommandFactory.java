package iotinfrastructure;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Consumer;

public class CommandFactory {

    private final Map<String, Command> map = new HashMap<>();
    private final Consumer<List<Class<?>>> factoryAdder = new FactoryAdder();

    private CommandFactory() {
    }

    private static class InstanceHolder {
        private static final CommandFactory instance = new CommandFactory();
    }

    public static CommandFactory getInstance() {
        return InstanceHolder.instance;
    }

    public boolean isServiceAvailable(String service) {
        return (null != map.get(service));
    }

    public Consumer<List<Class<?>>> getFactoryAdder() {
        return factoryAdder;
    }

    public void add(String key, Command command) {
        map.put(key, command);
    }

    public Command createCommand(String key) {
        return map.get(key);
    }

    private class FactoryAdder implements Consumer<List<Class<?>>> {
        @Override
        public void accept(List<Class<?>> classes) {
            for (Class<?> currClass : classes) {
                try {
                    CommandFactory.getInstance().add(currClass.getSimpleName(), (Command) currClass.newInstance());
                    System.out.println("Added new service: " + currClass.getSimpleName());
                } catch (InstantiationException | IllegalAccessException e) {
                    throw new RuntimeException(e);
                }
            }
        }
    }
}