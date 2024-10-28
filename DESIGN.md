Another encoding of Unicode is UTF-32, which encodes all Unicode code points in 4 bytes. For things like ASCII, the leading 3 bytes are all 0's. What are some tradeoffs between UTF-32 and UTF-8?

One benefit of having only UTF-32 is that there when you are doing operations on a string (character array) there is no need for extra checks for leading bits that signify a UTF8 multi-byte code point, or for continuation bits that signify the continuation of a UTF8 multi-byte code point. This means that the number represented by the four bytes in the UTF-32 character is the exact code point for a character. You could even represent more codepoints because there are no leading bits or continuation bits.The benefits of UTF-32 are more clear when dealing with emojis and non-english languages, since those are multi-byte codepoints. However, for smaller codepoints like the ones that make up ASCII, there is a lot of wasted memory used on the first three bytes of UTF-32, since ASCII can be represented by a single byte. The benefit of using UTF-8 is that the most common charachters (ASCII) can be represented very efficiently in a single byte, and even though multi-byte code points use  leading and continuation bits, UTF-8 can still represent tens of thousands of codepoints.

UTF-8 has a leading 10 on all the bytes past the first for multi-byte code points. This seems wasteful – if the encoding for 3 bytes were instead 1110XXXX XXXXXXXX XXXXXXXX (where X can be any bit), that would fit 20 bits, which is over a million code points worth of space, removing the need for a 4-byte encoding. What are some tradeoffs or reasons the leading 10 might be useful? Can you think of anything that could go wrong with some programs if the encoding didn't include this restriction on multi-byte code points?

It would be useful to not have the leading 10 because it means that there is more memory avalible to represent more charachters in less bytes, and it would save the coder some time because they wouldn't have to worry as much about masking. However, not having this leading 10 could make it very hard to differentiate between a continuation byte and a single byte charachter. For example, take this string: "🐱this is a cat". In memory, the first byte (most significant byte) of the cat's encoding would signify that the next 3 bytes make up the code point for that emoji by having the leading bits turned on. However, the last byte (or least significant byte) could have any bytes turned on or off. In fact, it could look identical to the next charachter in the string, the charachter 't'. If the coder made a mistake in offsetting bytes, or if old code didn't account for this change to UTF-8 encoding, then there would be no way to confirm whether or not you are looking at a continuation of the Emoji's codepoint, or if ou are just looking at the next charachter in the string.


Design Question 3: Resubmission