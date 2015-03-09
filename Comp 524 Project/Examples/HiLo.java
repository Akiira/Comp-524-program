import java.util.Scanner;

public class HiLo
{

   public static void main (String[] args)
   {
	   final int MAX = 100;
	      int num1, num2, target, count, guess;
	      Scanner scan = new Scanner(System.in);

	      System.out.println ("Enter two numbers between 1 and " + MAX);
	      num1 = Integer.parseInt(scan.nextLine());
	      num2 = Integer.parseInt(scan.nextLine());
	      target = num1 * num2;
	      System.out.println ("What is " + num1 + " x " + num2 +"?");

	        count = 0;
	        do
	        {
	           System.out.println();
	           System.out.print ("Enter your guess (0 to quit): ");
	           guess = Integer.parseInt(scan.nextLine());
	           count = count + 1;

	           if (guess == target)
	               System.out.println ("Right! Guesses ( " + count + ")");
	           else if (guess > target)
	               System.out.println ("Your guess was too HIGH.");
	           else if ((guess < num1) || (guess < num2))
	               System.out.println ("You are not even trying.");
	           else if (guess != 0)
	               System.out.println ("Your guess was too LOW.");
	        } while (guess != target && guess != 0);
   }
}
