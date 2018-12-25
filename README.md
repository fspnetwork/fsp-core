# Financial Securities Protocol

Root repository of EOS implementation of [Financial Securities Protocol](https://www.fsp.network/) - an open-source blueprint for issue and life-cycle management of self-sovereign securities. First and foremost, FSP helps mitigate the undesired side-effects of "paper dependency" and increase the velocity, efficiency and liquidity of represented financial instruments. 

## `fsp.network`

Consisting of a number of base modules, `fsp.network` provides a foundation of Financial Securities Protocol.

### `fsp.security`

The security contract defines the most generic of methods required for life-cycle management of securities. Where appropriate, instances of `fsp.security` may call `fsp.regulator` to validate various operations, such as sending, issuing and receiving. 


### `fsp.regulator`

The regulator is a module responsible for constraining various actions, such that FSP entities can be self-sovereign. Methods of `fsp.regulator` implementations get called in various methods of `fsp.security`. The relationship between security and regulator contracts is intended to be surjective, meaning that more than one security contract may be assigned to a single regulator contract. 

### `fsp.registry`

Directly supporting `fsp.regulator` contract, `fsp.registry` provides an entry point for counter party verification, thus mapping  real-world correspondance to entities living on the prtocol. In addition, Know Your Customer and Anti-Money Laundering (KYC / AML) stages of on-boarding are performed by validated partner parties off-chain in relevant jurisdictions. A party will have access to a relevant suit of FSP functionality once it gets successfully on-boarded.

### `fsp.communication`

The communication component of FSP allows for prompt communication between various modules and the users. Topics of communication can vary according to the event that triggers the communication and side-effects such as emails and other correpondance. Sensitive data may be handled via public-private key cryptography, such that only recipient of data can read view it.

### `fsp.exchange`

Point of liquidity for instances of `fsp.security`. Provides a suit of exchange operations, which rely on any securities exchange constraints defined within `fsp.regulator` contract. 

The following diagram suggests the intended usage and relationship between modules:

![alt text](http://funkyimg.com/i/2Py4S.png)

## FS2P

Financial Securities Protocol Proposals (FS2P) are encouraged to be submitted in order to refine functionality of exisiting modules. Widely accepted proposals will be implemented. 

## Contribution

If you would like to contribute to the protocol, issue a PR.  