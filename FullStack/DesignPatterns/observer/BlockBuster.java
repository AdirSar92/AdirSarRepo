/**
 * @Author: Asaf Madari
 * @Reviewer: Tzach Halfon
 * @Date: 30/08/2022
 * @Description: Observer design pattern implementation
 */

package il.co.ilrd.designpatterns.observer;

import org.junit.jupiter.api.Test;

public class BlockBuster {
    @Test
    void testPublisher() {
        Publisher<String> horrorBlockBuster = new Publisher<>();
        Publisher<String> comedyBlockBuster = new Publisher<>();
        Observer<String> horrorLover = new Observer<>(s -> System.out.println("THIS IS SCAAAARY: " + s), () -> System.out.println("goodbye Horror!"));
        Observer<String> comedyLover = new Observer<>(s -> System.out.println("ROFL: " + s), () -> System.out.println("goodbye Comedy!"));

        System.out.println("======================== Test 1 =========================");

        horrorLover.register(horrorBlockBuster);
        horrorBlockBuster.publish("Orphan");
        System.out.println(horrorLover.getData() + " was not scary enough, unregister!");
        horrorLover.unregister();
        horrorLover.register(horrorBlockBuster);

        comedyLover.register(comedyBlockBuster);
        comedyBlockBuster.publish("Scary movie 2 :)");

        System.out.println(("Blockbuster's close. please return all films!"));
        comedyBlockBuster.close();
        horrorBlockBuster.close();
    }

    @Test
    void testWindow() {
        System.out.println("======================== Test 2 =========================");

        Publisher<String> publisher = new Publisher<>();
        ViewWindow<String> viewWindow = new ViewWindow<>(s -> System.out.println(s + " viewWindow!"), () -> System.out.println("Goodbye viewWindow"));
        DataModel<String> dataModel = new DataModel<>(s -> System.out.println(s + " dataModel!"), () -> System.out.println("Goodbye viewWindow"));
        ControlWindow<String> controlWindow = new ControlWindow<>(s -> System.out.println(s + " controlWindow!"), () -> System.out.println("Goodbye viewWindow"));
        for (int i = 0; i < 30; i++) {
            if (0 == i % 5) viewWindow.register(publisher);
            if (0 == i % 10) dataModel.register(publisher);
            if (0 == i % 15) controlWindow.register(publisher);
            if (0 == i % 20) viewWindow.unregister();
            if (0 == i % 25) dataModel.unregister();
            if (0 == i % 30) controlWindow.unregister();
            if (0 == i % 5) publisher.publish("Hello all");
        }

    }
}
