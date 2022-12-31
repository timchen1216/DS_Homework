### Description
Simplifies dense cash flows

### Skills nurtured:
  - Heap & Graph.
  - Splitwise algorithm to remove unnecessary edges(cash flow) of graph.

### algo
  - 1. Calculate the amount owed by each person.
  - 2. Debit = min(amount), credit = max(amount), settlement_amount = min{|debit|, credit}
  - 3. Settlement amount paid by the debtor to the creditor.
  - 4. Go back to step 2 until the amount owed is paid in full.

### Time complexity
  - 1. Input transactions and add in map(Red-Black tree). O(n) * O(logn)  n:number of transactions
  - 2. Add amount to multiset(B tree). O(n) * O(logn)
  - 3. Settlement amount paid by the debtor to the creditor. O(1)
  - 4. Go back to step 3 until the amount owed is paid in full. O(n)

Time complexity = O(nlogn) + O(nlogn) + O(1) * O(n) = O(nlogn)