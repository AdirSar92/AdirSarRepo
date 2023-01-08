package iotinfrastructure.parsers;

public interface Parser<E> {
    public E parse(String request);
    public boolean isRequestValid(String[] split);
}
