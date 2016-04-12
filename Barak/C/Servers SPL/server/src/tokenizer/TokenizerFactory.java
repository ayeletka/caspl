package tokenizer;

public interface TokenizerFactory<T> {
   @SuppressWarnings("rawtypes")
Tokenizer create(boolean reactorMethod);
}
