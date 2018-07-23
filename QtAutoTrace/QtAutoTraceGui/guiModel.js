var Steps = Object.freeze({
    HOME: 0,
    LOADING: 1,
    RESULTS: 2
});

/**
 * Ctor for GuidModel object.
 * @param {object} forms
 * @returns {GuiModel}
 */
function newGuiModel(forms) {
    //#region Private

    var _error = null;
    var _step = Steps.HOME

    //#endregion

    //#region Public
    this.error = function(error) {
        if (error) {
            this.step(Steps.RESULTS);
            _error = error;
            return;
        } else {
            return _error;
        }
    }

    this.hasError = function() {
        return this.error() !== null;
    }

    this.clearError = function() {
        _error = null;
    }

    this.step = function(step) {
        if (step) {
            if (this.hasError()) {
                // Can't navigate to any step while there is an error.
                throw new Error("Need to clear errors before navigating to a "
                    + "new step.");
            }

            if (!(step in Steps)) {
                throw new Error("Steps doesn't contain '" + step + "'.");
            } else {
                _step = step;
            }
        } else {
            return _step;
        }
    }
    //#endregion
    return model;
}
