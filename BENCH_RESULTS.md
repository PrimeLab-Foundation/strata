| Library       | Dataset      | Query                                      | Parse (ms) | Query (ms) | Total (ms) | MB/s | Peak RSS (MB) | Result Count |
| ------------- | ------------ | ------------------------------------------ | ---------- | ---------- | ---------- | ---- | ------------- | ------------ |
| turbojsonpath | users.json   | $.users\[\*\].id                           | 0          | 31         | 55         | 92   | 106           | 5000         |
| turbojsonpath | users.json   | $.users\[*\].orders\[*\].items\[\*\].price | 0          | 52         | 73         | 69   | 125           | 62331        |
| turbojsonpath | users.json   | $.users\[?(@.age>30)\]                     | 0          | 41         | 63         | 81   | 202           | 3999         |
| turbojsonpath | users.ndjson | $.id                                       | 0          | 58         | 66         | 77   | 212           | 5000         |
| turbojsonpath | users.ndjson | $.orders\[*\].items\[*\].price             | 0          | 87         | 94         | 53   | 218           | 62331        |
| turbojsonpath | users.ndjson | $\[?(@.age>30)\]                           | 0          | 55         | 63         | 80   | 218           | 0            |
| ijson         | users.json   | $.users\[\*\].id                           | 0          | 64         | 86         | 58   | 219           | 0            |
| ijson         | users.json   | $.users\[*\].orders\[*\].items\[\*\].price | 0          | 62         | 84         | 60   | 219           | 0            |
| ijson         | users.json   | $..price                                   | 0          | 63         | 86         | 59   | 219           | 0            |
| ijson         | users.json   | $.users\[?(@.age>30)\]                     | 0          | 64         | 84         | 60   | 219           | 0            |
| ijson         | users.json   | $..orders\[?(@.status=="shipped")\]        | 0          | 67         | 89         | 57   | 219           | 0            |
