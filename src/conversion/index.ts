const CHAR_CAP_DEFAULT: Number = 50000;

/**
  * Returns max allowed value for chars. Although, you can change this by giving
  * an argument, this should suffice for now.
  * @example
  * const charCap = getMaxAllowedCharValue(); // charCap will be 50000
  */

function getMaxAllowedCharValue(allowed: Number = CHAR_CAP_DEFAULT) {
  return allowed;
}

/**
  * Calculates distance between characters to find the appropriate distance
  * to be used in converting insane values back to the ascii standard.
  * @example
  * const charDist = calculateCharDistance('', 'L'); // charDist will be 58849
  */
function calculateCharDistance(highChar: String = '', lowChar: String = 'L'): Number {
// 'L'.charCodeAt(0) - ('L'.charCodeAt(0) - 'L'.charCodeAt(1));
  const charDist = highChar.charCodeAt(0) - lowChar.charCodeAt(0);
  return charDist;
}

module.exports = {
  getMaxAllowedCharValue,
  calculateCharDistance,
};
