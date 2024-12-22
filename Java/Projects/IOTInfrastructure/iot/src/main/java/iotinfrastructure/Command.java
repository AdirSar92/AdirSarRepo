package iotinfrastructure;

@FunctionalInterface
public interface Command {
    public String execute(String data);
}
