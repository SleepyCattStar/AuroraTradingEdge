### Generating Random Float for fake market-data generation(mock)

```
std::random_device rd;
std::mt19937 gen(rd());
```

// Define a decimal distribution between 0.0 and 1.0 (inclusive of 0.0, exclusive of 1.0)
std::uniform_real_distribution<double> distrib(0.0, 1.0);